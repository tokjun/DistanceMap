/*=========================================================================

  Program:   Vector Image Scaling Class for ITK.
  Language:  C++
  Copyright (c) Brigham and Women's Hospital. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkScaleVectorImageFilter_h
#define __itkScaleVectorImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkCenteredAffineTransform.h"

namespace itk
{
/** \class ScaleVectorImageFilter
 * \brief 
 * \ingroup IntensityImageFilters TensorObjects
 *
 */
  
template < typename  TInput, typename TOutput  >
class ITK_EXPORT ScaleVectorImageFilter : public
ImageToImageFilter< TInput, TOutput >
{
public:
  /** Standard class typedefs. */
  typedef ScaleVectorImageFilter Self;
  typedef ImageToImageFilter<
          TInput,
          TOutput >                               Superclass;
  typedef SmartPointer<Self>                      Pointer;
  typedef SmartPointer<const Self>                ConstPointer;
  
  typedef typename Superclass::InputImageType            InputImageType;
  typedef typename Superclass::OutputImageType           OutputImageType;
  typedef typename InputImageType::PixelType             InputPixelType;
  typedef typename OutputImageType::PixelType            OutputPixelType;

  typedef itk::Vector< double, InputImageType::ImageDimension > VectorType;

  /** Run-time type information (and related methods).   */
  itkTypeMacro( ScaleVectorImageFilter, ImageToImageFilter );

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  inline void SetScaleVector(VectorType& vector)
  {
    for (int i = 0; i < InputImageType::ImageDimension; i ++)
      {
      this->m_ScaleVector[i] = vector[i];
      }
  }

  inline void GetScaleVector(VectorType& vector)
  {
    for (int i = 0; i < i; i ++)
      {
      vector[i] = this->m_ScaleVector[i];
      }
  }

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(DoubleConvertibleToOutputCheck,
                  (Concept::Convertible<double, OutputPixelType>));
  /** End concept checking */
#endif

protected:
  ScaleVectorImageFilter();
  ~ScaleVectorImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;
  
  /** Generate Data */
  void GenerateData( void );

private:
  ScaleVectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  VectorType m_ScaleVector;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkScaleVectorImageFilter.txx"
#endif
  
#endif
